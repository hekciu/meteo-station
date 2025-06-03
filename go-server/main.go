package main

import (
	"errors"
	"fmt"
	"io"
	"net/http"
    "net/http/httputil"
	"os"
    "database/sql"
    "encoding/json"
    "crypto/subtle"
    "crypto/sha256"
    _ "github.com/lib/pq"
)


type Pms5003MeasurementDTO struct {
    DeviceTimestamp uint32 `json:"DeviceTimestamp"`
    DeviceName string `json:"DeviceName"`
    Pm10Standard uint8 `json:"Pm10Standard"`
    Pm25Standard uint8 `json:"Pm25Standard"`
    Pm100Standard uint8 `json:"Pm100Standard"`
}


type LocationDTO struct {
    DeviceName string `json:"DeviceName"`
    Latitude float32 `json:"Latitude"`
    Longitude float32 `json:"Longitude"`
}


func _connectToDb() *sql.DB {
    pgPort := os.Getenv("PG_PORT")
    pgHost := os.Getenv("PG_HOST")
    pgUser := os.Getenv("PG_USER")
    pgPassword := os.Getenv("PG_PASSWORD")
    pgDbname := os.Getenv("PG_DBNAME")

    envs := [...]string{
        pgPort,
        pgHost,
        pgUser,
        pgPassword,
        pgDbname,
    }

    for _, el := range envs {
        if len(el) == 0 {
            fmt.Fprint(os.Stderr, "some environment variables are missing\n")
            return nil
        }
    }

	connString := fmt.Sprintf("user=%s dbname=%s port=%s password=%s host=%s", pgUser, pgDbname, pgPort, pgPassword, pgHost)

	db, db_err := sql.Open("postgres", connString)

	if db_err != nil {
        fmt.Printf("could not create database connection, got: %s\n", db_err)
        return nil
	}

    db_err = db.Ping()

    if db_err != nil {
        fmt.Printf("could not establish database connection, got: %s\n", db_err)
        return nil
    }

    return db
}


func _auth(r * http.Request) bool {
    thisApiPassword := os.Getenv("THIS_API_PASSWORD")
    thisApiUsername := os.Getenv("THIS_API_USERNAME")

    envs := [...]string{
        thisApiPassword,
        thisApiUsername,
    }

    for _, el := range envs {
        if len(el) == 0 {
            fmt.Fprint(os.Stderr, "some environment variables are missing\n")
            return false
        }
    }

    username, password, ok := r.BasicAuth()

    if !ok {
        return false
    }

    usernameHash := sha256.Sum256([]byte(username))
    passwordHash := sha256.Sum256([]byte(password))
    usernameHashExpected := sha256.Sum256([]byte(thisApiUsername))
    passwordHashExpected := sha256.Sum256([]byte(thisApiPassword))

    usernameMatch := (subtle.ConstantTimeCompare(usernameHash[:], usernameHashExpected[:]) == 1)
    passwordMatch := (subtle.ConstantTimeCompare(passwordHash[:], passwordHashExpected[:]) == 1)

    return usernameMatch && passwordMatch
}


func postMeasurements(w http.ResponseWriter, r * http.Request) {
    reqDump, err := httputil.DumpRequest(r, true)

    if err != nil {
        http.Error(w, "an error has occured\n", http.StatusInternalServerError)
    }

    fmt.Printf("got request:\n%s\n", string(reqDump));

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    if r.Body == nil {
        fmt.Fprintf(os.Stderr, "empty body\n");
        http.Error(w, "empty body\n", http.StatusBadRequest)
        return
    }

    decoder := json.NewDecoder(r.Body)

    measurement := Pms5003MeasurementDTO{}

    err = decoder.Decode(&measurement)

    if err != nil {
        fmt.Fprintf(os.Stderr, "error parsing body got: %s\n", err);
        http.Error(w, "error parsing body\n", http.StatusBadRequest)
        return
    }

    db := _connectToDb()

    if db == nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    query := fmt.Sprintf(`INSERT INTO pms5003_measurements(
        device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard)
        VALUES(to_timestamp(%d), '%s', %d, %d, %d)`,
        measurement.DeviceTimestamp,
        measurement.DeviceName,
        measurement.Pm10Standard,
        measurement.Pm25Standard,
        measurement.Pm100Standard)

    _, err = db.Query(query)

    if err != nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    io.WriteString(w, "Ok")
}


func getMeasurements(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("GET /pms5003\n")

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    db := _connectToDb()

    defer db.Close()

    if db == nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    from := r.URL.Query().Get("from");
    to := r.URL.Query().Get("to");

    if len(from) == 0 || len(to) == 0 {
        http.Error(w, "missing query params\n", http.StatusInternalServerError)
        return
    }

    query := fmt.Sprintf("SELECT * FROM pms5003_measurements WHERE device_timestamp > to_timestamp(%d) and device_timestamp < to_timestamp(%d)", from, to)

    rows, err := db.Query(query)

    if err != nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    defer rows.Close()

    var output []Pms5003MeasurementDTO

    for rows.Next() {
        rowData :=  Pms5003MeasurementDTO{}

        rows.Scan(&rowData.DeviceTimestamp,
                    &rowData.DeviceName,
                    &rowData.Pm10Standard,
                    &rowData.Pm25Standard,
                    &rowData.Pm100Standard)

        output = append(output, rowData)
    }

    marshalled, err := json.Marshal(output)

    if err != nil {
        http.Error(w, "as error has occured\n", http.StatusInternalServerError)
        return
    }

    io.WriteString(w, string(marshalled))
}


func getLocation(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("GET /technical/location\n")

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    db := _connectToDb()

    defer db.Close()

    if db == nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    deviceName := r.URL.Query().Get("device_name");

    if len(deviceName) == 0 {
        http.Error(w, "missing device_name param\n", http.StatusInternalServerError)
        return
    }

    query := fmt.Sprintf("SELECT * FROM location WHERE device_name='%s'", deviceName);

    rows, err := db.Query(query)

    if err != nil {
        http.Error(w, "database error\n", http.StatusInternalServerError)
        return
    }

    defer rows.Close()

    output := LocationDTO{}

    var numRows int = 0
    // we will just get the last one, this is awful but will work for now
    for rows.Next() {
        rows.Scan(&output.DeviceName,
                    &output.Latitude,
                    &output.Longitude)

        numRows++;
    }

    if numRows == 0 {
        http.Error(w, "did not find location for provided device_name\n", http.StatusInternalServerError)
        return
    }

    marshalled, err := json.Marshal(output)

    if err != nil {
        http.Error(w, "as error has occured\n", http.StatusInternalServerError)
        return
    }

    io.WriteString(w, string(marshalled))
}


func putLocation(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("PUT /technical/location\n")

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    db := _connectToDb()

    defer db.Close()

    if db == nil {
        http.Error(w, "cound not connect to database\n", http.StatusInternalServerError)
        return
    }

    if r.Body == nil {
        fmt.Fprintf(os.Stderr, "empty body\n");
        http.Error(w, "empty body\n", http.StatusBadRequest)
        return
    }

    decoder := json.NewDecoder(r.Body)

    loc := LocationDTO{}

    err := decoder.Decode(&loc)

    if err != nil {
        fmt.Fprintf(os.Stderr, "error parsing body got: %s\n", err);
        http.Error(w, "error parsing body\n", http.StatusBadRequest)
        return
    }

    query := fmt.Sprintf(`INSERT INTO location(
        device_name, latitude, longitude)
        VALUES('%s', %f, %f)`,
        loc.DeviceName,
        loc.Latitude,
        loc.Longitude)

    _, err = db.Query(query)

    if err != nil {
        http.Error(w, "could not insert data into database\n", http.StatusInternalServerError)
        return
    }

    io.WriteString(w, "Ok")
}


func getDevices(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("GET /devices\n")

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    db := _connectToDb()

    defer db.Close()

    if db == nil {
        http.Error(w, "could not connect to database\n", http.StatusInternalServerError)
        return
    }

    query := "SELECT device_name FROM location";

    rows, err := db.Query(query)

    if err != nil {
        http.Error(w, "could not execute query\n", http.StatusInternalServerError)
        return
    }

    defer rows.Close()

    output := []string{}

    for rows.Next() {
        var deviceName string

        rows.Scan(&deviceName)

        output = append(output, deviceName)
    }

    marshalled, err := json.Marshal(output)

    if err != nil {
        http.Error(w, "as error has occured\n", http.StatusInternalServerError)
        return
    }

    io.WriteString(w, string(marshalled))
}



func handlePms5003Measurements(w http.ResponseWriter, r * http.Request) {
    if r.Method == "GET" {
        getMeasurements(w, r)
    } else if r.Method == "POST" {
        postMeasurements(w, r)
    } else {
        io.WriteString(w, "method not supported\n")
    }
}


func handleLocation(w http.ResponseWriter, r * http.Request) {
    if r.Method == "GET" {
        getLocation(w, r)
    } else if r.Method == "PUT" {
        putLocation(w, r)
    } else {
        io.WriteString(w, "method not supported\n")
    }
}


func handleDevices(w http.ResponseWriter, r * http.Request) {
    if r.Method == "GET" {
        getDevices(w, r)
    } else {
        io.WriteString(w, "method not supported\n")
    }
}


func main() {
    http.HandleFunc("/pms5003", handlePms5003Measurements);
    http.HandleFunc("/devices", handleDevices);
    http.HandleFunc("/technical/location", handleLocation);

    port := os.Getenv("PORT")

    if len(port) == 0 {
        fmt.Fprint(os.Stderr, "port environment variable is missing\n")
        return;
    }

    fmt.Printf("starting server at %s...\n", port)
    server_err := http.ListenAndServe(":" + port, nil)

    if errors.Is(server_err, http.ErrServerClosed) {
        fmt.Printf("server closed\n");
        os.Exit(1);
    } else if server_err != nil {
        fmt.Printf("error starting server: %s\n", server_err);
        os.Exit(1);
    }
}  	

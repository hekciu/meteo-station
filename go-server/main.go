package main

import (
	"errors"
    "time"
	"fmt"
	"io"
	"net/http"
	"os"
    "database/sql"
    "encoding/json"
    "crypto/subtle"
    "crypto/sha256"
    _ "github.com/lib/pq"
)


type Pms5003MeasurementDTO struct {
    DeviceTimestamp time.Time `json:"DeviceTimestamp"`
    DeviceName string `json:"DeviceName"`
    Pm10Standard uint8 `json:"Pm10Standard"`
    Pm25Standard uint8 `json:"Pm25Standard"`
    Pm100Standard uint8 `json:"Pm100Standard"`
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
    fmt.Printf("POST /pms5003\n")

    if (!_auth(r)) {
        http.Error(w, "unauthorized\n", http.StatusUnauthorized)
        return
    }

    if r.Body == nil {
        http.Error(w, "empty body\n", http.StatusBadRequest)
        return
    }

    decoder := json.NewDecoder(r.Body)

    measurement := Pms5003MeasurementDTO{}

    err := decoder.Decode(&measurement)

    if err != nil {
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
        measurement.DeviceTimestamp.Unix(),
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

    rows, err := db.Query("SELECT * FROM pms5003_measurements")

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


func handlePms5003Measurements(w http.ResponseWriter, r * http.Request) {
    if r.Method == "GET" {
        getMeasurements(w, r)
    } else if r.Method == "POST" {
        postMeasurements(w, r)
    } else {
        io.WriteString(w, "method not supported\n")
    }
}


func main() {

    http.HandleFunc("/pms5003", handlePms5003Measurements);

    fmt.Printf("starting server...\n")
    server_err := http.ListenAndServe(":3333", nil)

    if errors.Is(server_err, http.ErrServerClosed) {
        fmt.Printf("server closed\n");
        os.Exit(1);
    } else if server_err != nil {
        fmt.Printf("error starting server: %s\n", server_err);
        os.Exit(1);
    }
}  	

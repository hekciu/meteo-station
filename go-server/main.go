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
    port := os.Getenv("PORT")
    thisApiPassword := os.Getenv("THIS_API_PASSWORD")
    thisApiUsername := os.Getenv("THIS_API_USERNAME")
    pgPort := os.Getenv("PG_PORT")
    pgHost := os.Getenv("PG_HOST")
    pgUser := os.Getenv("PG_USER")
    pgPassword := os.Getenv("PG_PASSWORD")
    pgDbname := os.Getenv("PG_DBNAME")

    envs := [...]string{
        port,
        thisApiPassword,
        thisApiUsername,
        pgPort,
        pgHost,
        pgUser,
        pgPassword,
        pgDbname,
    }

    for _, el := range envs {
        if len(el) == 0 {
            fmt.Printf("some environment variables are missing\n")
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


func postMeasurements(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("POST /pms5003\n")

    if r.Body == nil {
        io.WriteString(w, "empty body\n")
        return
    }

    decoder := json.NewDecoder(r.Body)

    measurement := Pms5003MeasurementDTO{}

    err := decoder.Decode(&measurement)

    if err != nil {
        io.WriteString(w, "error parsing body\n")
        return
    }

    db := _connectToDb()

    query := fmt.Sprintf(`INSERT INTO pms5003_measurements(
        device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard)
        VALUES(to_timestamp(%d), '%s', %d, %d, %d)`,
        measurement.DeviceTimestamp.Unix(),
        measurement.DeviceName,
        measurement.Pm10Standard,
        measurement.Pm25Standard,
        measurement.Pm100Standard)

    fmt.Printf(query)

    _, err = db.Query(query)

    if err != nil {
        io.WriteString(w, "database error\n")
        return
    }

    io.WriteString(w, "Ok")
}


func getMeasurements(w http.ResponseWriter, r * http.Request) {
    fmt.Printf("GET /pms5003\n")

    db := _connectToDb()

    defer db.Close()

    if db == nil {
        io.WriteString(w, "database error!\n")
        return
    }

    rows, err := db.Query("SELECT * FROM pms5003_measurements")

    if err != nil {
        io.WriteString(w, "database error!\n")
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
        io.WriteString(w, "an error has occured\n")
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

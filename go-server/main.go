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


type Pms5003Measurement struct {
    Timestamp time.Time
    DeviceName string
    Pm10Standard uint8
    Pm25Standard uint8
    Pm100Standard uint8
}


func getMeasurements(w http.ResponseWriter, r *http.Request) {
    if r->Method != "GET" {
        io.WriteString(w, "method not supported\n")
        return
    }

    fmt.Printf("got /measurements request\n")

    rows, err := db.Query("SELECT * FROM pms5003_measurements")

    if err != nil {
        io.WriteString(w, "database error!\n")
        return
    }

    defer rows.Close()

    var output []Pms5003Measurement

    for rows.Next() {
        rowData :=  Pms5003Measurement{}

        rows.Scan(&rowData.Timestamp,
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


func main() {
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
            os.Exit(1);
        }
    }

	connString := fmt.Sprintf("user=%s dbname=%s port=%s password=%s host=%s", pgUser, pgDbname, pgPort, pgPassword, pgHost)

	db, db_err := sql.Open("postgres", connString)

	if db_err != nil {
        fmt.Printf("could not create database connection, got: %s\n", db_err)
        os.Exit(1)
	}

    db_err = db.Ping()

    if db_err != nil {
        fmt.Printf("could not establish database connection, got: %s\n", db_err)
        os.Exit(1)
    }

    http.HandleFunc("/measurements", getMeasurements);

    server_err := http.ListenAndServe(":3333", nil)

    if errors.Is(server_err, http.ErrServerClosed) {
        fmt.Printf("server closed\n");
    } else if server_err != nil {
        fmt.Printf("error starting server: %s\n", server_err);
        os.Exit(1);
    }
}  	

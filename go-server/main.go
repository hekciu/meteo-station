package main

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
    "database/sql"
)


func getMeasurements(w http.ResponseWriter, r *http.Request) {
    fmt.Printf("got /measurements request\n")
    io.WriteString(w, "Hello, HTTP!\n")
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

    http.HandleFunc("/measurements", getMeasurements);

	connStr := "user=pqgotest dbname=pqgotest sslmode=verify-full"
	_, db_err := sql.Open("postgres", connStr)

	if db_err != nil {
        fmt.Printf("could not connect to the database, got: %s\n", db_err)
        os.Exit(1)
	} else {
        fmt.Printf("Successfully connected to postgres database\n")
    }

    server_err := http.ListenAndServe(":3333", nil)

    if errors.Is(server_err, http.ErrServerClosed) {
        fmt.Printf("server closed\n");
    } else if server_err != nil {
        fmt.Printf("error starting server: %s\n", server_err);
        os.Exit(1);
    }
}  	

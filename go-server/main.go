package main

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
    "database/sql"
    _ "github.com/lib/pq"
)




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

    http.HandleFunc("/measurements", func (w http.ResponseWriter, r *http.Request) {
        fmt.Printf("got /measurements request\n")

        rows, err := db.Query("SELECT * FROM pms5003_measurements")

        if err != nil {
            io.WriteString(w, "database error!\n")
        }

        defer rows.Close()

        output := ""

        for rows.Next() {

        }

        io.WriteString(w, "Hello, HTTP!\n")
    });

    server_err := http.ListenAndServe(":3333", nil)

    if errors.Is(server_err, http.ErrServerClosed) {
        fmt.Printf("server closed\n");
    } else if server_err != nil {
        fmt.Printf("error starting server: %s\n", server_err);
        os.Exit(1);
    }
}  	

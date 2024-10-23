#include <stdint.h>
#include "postgresql/libpq-fe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_database.h"


void _createConnInfoFromEnvs(char ** output) {
    char * port = getenv("PG_PORT");
    char * host = getenv("PG_HOST");
    char * user = getenv("PG_USER");
    char * password = getenv("PG_PASSWORD");
    char * dbname = getenv("PG_DBNAME");

    if (port == NULL) {
        fprintf(stderr, "PG_PORT env var missing\n");
        exit(1);
    }
    if (host == NULL) {
        fprintf(stderr, "PG_HOST env var missing\n");
        exit(1);
    }
    if (user == NULL) {
        fprintf(stderr, "PG_USER env var missing\n");
        exit(1);
    }
    if (password == NULL) {
        fprintf(stderr, "PG_PASSWORD env var missing\n");
        exit(1);
    }
    if (dbname == NULL) {
        fprintf(stderr, "PG_DBNAME env var missing\n");
        exit(1);
    }

    const char * schema = "user=%s dbname=%s password=%s host=%s port=%s"; 
    int outputSize = strlen(schema) - 10 + strlen(port) + strlen(host) + strlen(user) + strlen(password) + strlen(dbname) + 1;
    *output = malloc(outputSize);

    snprintf(*output, outputSize, schema, user, dbname, password, host, port);
}


int get_PMS5003_measurements(uint64_t timestampFrom, uint64_t timestampTo, char ** output) {
    char * conninfo;
    PGconn * conn;
    PGresult * res;
    int nTuples, nFields;

    _createConnInfoFromEnvs(&conninfo);

    conn = PQconnectdb(conninfo);
    free(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "connection error: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 500;
    }

    
    char * query = malloc(MAX_PG_QUERY_SIZE);
    snprintf(query, MAX_PG_QUERY_SIZE, "SELECT * FROM pms5003_measurements WHERE device_timestamp > to_timestamp(%ld) and device_timestamp < to_timestamp(%ld)", timestampFrom, timestampTo);
    res = PQexec(conn, query);
    ExecStatusType execStatus = PQresultStatus(res);
    printf("pg query exec status: %d\n", execStatus);
    free(query);
    printf("ExecStatusType: %d\n", execStatus);

    return 0;
}


int insert_PMS5003_measurement(char * tableName, char * data) {
    
}

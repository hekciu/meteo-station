#include <stdint.h>
#include "postgresql/libpq-fe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <_database.h>


int _createConnInfoFromEnvs() {

}


size_t get_measurements(uint64_t timestampFrom, uint64_t timestampTo) {
    const char * conninfo;
    PGconn * conn;
    PGresult * res;
    int nTuples, nFields;

    if (_createConnInfoFromEnvs(&conninfo) != 0) {
        fprintf(stderr, "could not create pg conninfo from env\n");
        return -1;
    }

    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "connection error: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }

    
    char * query = malloc(MAX_PG_QUERY_SIZE);
    snprintf(query, MAX_PG_QUERY_SIZE, "SELECT * FROM pms5003_measurements");
    res = PQexec(conn, query);
    ExecStatusType execStatus = PQresultStatus(res);
    free(query);
    printf("ExecStatusType: %d\n", execStatus);
}


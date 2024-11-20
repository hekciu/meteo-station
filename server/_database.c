#define _XOPEN_SOURCE /* This is needed to stop gcc complaining about strptime function */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "_database.h"
#include "postgresql/libpq-fe.h"
#include "../common/json_array_from_anything.h"
#include "_parse.h"


#define PMS5003_JSON_MAX_SIZE 1024
#define PG_TIMESTAMP_MAX_SIZE 19
#define EPOCH_MAX_SIZE 10

const char * DEVICE_TIMESTAMP_FN = "device_timestamp";
const char * DEVICE_NAME_FN = "device_name";
const char * PM10_STANDARD_FN = "pm10_standard";
const char * PM25_STANDARD_FN = "pm25_standard";
const char * PM100_STANDARD_FN = "pm100_standard";


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


int _extract_tuples(PGresult * res, int (*createTupleJson)(PGresult * res, int nRow, char ** output), char ** output) {
    ExecStatusType execStatus = PQresultStatus(res);
    int outputCode = 0;

    char * EMPTY_OUTPUT = "[]";

    switch (execStatus) {
    case PGRES_COMMAND_OK:
        *output = malloc(strlen(EMPTY_OUTPUT) + 1);
        sprintf(*output, "%s", EMPTY_OUTPUT);
        break;
    case PGRES_TUPLES_OK:
    case PGRES_SINGLE_TUPLE:
        ; // <- compiler needs that
        bool returnEmpty = false;

        if (PQntuples(res) == 0) {
            returnEmpty = true; 
        } 

        for (int i = 0; i < PQntuples(res); i++) {
            char * outputTuple = NULL;
            if ((*createTupleJson)(res, i, &outputTuple) != 0) {
                outputCode = 1;
                returnEmpty = true;
                break;
            } else {
                jsonArrayFromAnything(output, outputTuple);
            }
        }

        if (!returnEmpty) {
            jsonArrayFromAnything(output, NULL);
        } else {
            *output = malloc(strlen(EMPTY_OUTPUT) + 1);
            sprintf(*output, "%s", EMPTY_OUTPUT);
        }
        break;
    default:
        *output = malloc(strlen(EMPTY_OUTPUT) + 1);
        sprintf(*output, "%s", EMPTY_OUTPUT);
        outputCode = 1;
        break;
    } 

    return outputCode;
}


int _createPMS5003TupleJson(PGresult * res, int nRow, char ** output) {
    uint64_t deviceTimestamp; 
    char * deviceName = NULL; 
    uint16_t pm10Standard;
    uint16_t pm25Standard;
    uint16_t pm100Standard;

    for (int i = 0; i < PQnfields(res); i++) {
        char * fieldName = PQfname(res, i); 
        // TODO: check if strtol/strtoll does not create memory leaks when input is wrong
        if (strcmp(DEVICE_TIMESTAMP_FN, fieldName) == 0) {
            char * measurementDate = PQgetvalue(res, nRow, i);
            struct tm measurementTime = {};
            strptime(measurementDate, "%Y-%m-%d %H:%M:%S", &measurementTime);
            measurementTime.tm_hour += 1; //TODO: This is a WORKAROUND I don't know why it works that way
            time_t epoch = mktime(&measurementTime);
            deviceTimestamp = (uint64_t)epoch;
        } else if (strcmp(DEVICE_NAME_FN, fieldName) == 0) {
            deviceName = PQgetvalue(res, nRow, i);
        } else if (strcmp(PM10_STANDARD_FN, fieldName) == 0) {
            pm10Standard = (uint16_t)strtol(PQgetvalue(res, nRow, i), NULL, 10);
        } else if (strcmp(PM25_STANDARD_FN, fieldName) == 0) {
            pm25Standard = (uint16_t)strtol(PQgetvalue(res, nRow, i), NULL, 10);
        } else if (strcmp(PM100_STANDARD_FN, fieldName) == 0) {
            pm100Standard = (uint16_t)strtol(PQgetvalue(res, nRow, i), NULL, 10);
        } else {
            fprintf(stderr, "got unsupported field name from pg database: %s\n", fieldName);
            return 1;
        }
    }

    const char * dataFormat = "{ \"%s\": %lld, \"%s\": \"%s\", \"%s\": %d, \"%s\": %d, \"%s\": %d }";
    char * json = malloc(PMS5003_JSON_MAX_SIZE);

    snprintf(json,
        PMS5003_JSON_MAX_SIZE,
        dataFormat,
        DEVICE_TIMESTAMP_FN,
        deviceTimestamp,
        DEVICE_NAME_FN,
        deviceName,
        PM10_STANDARD_FN,
        pm10Standard,
        PM25_STANDARD_FN,
        pm25Standard,
        PM100_STANDARD_FN,
        pm100Standard);
    
    *output = malloc(strlen(json) + 1);
    memcpy(*output, json, strlen(json));
    *(*output + strlen(json)) = '\0';

    free(json);
    return 0;
} 


int get_PMS5003_measurements(uint64_t timestampFrom, uint64_t timestampTo, char ** output) {
    char * conninfo;
    PGconn * conn;
    PGresult * res;

    _createConnInfoFromEnvs(&conninfo);

    conn = PQconnectdb(conninfo);
    free(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "connection error: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    
    char * query = malloc(MAX_PG_QUERY_SIZE);
    snprintf(query, MAX_PG_QUERY_SIZE, "SELECT * FROM pms5003_measurements WHERE device_timestamp > to_timestamp(%ld) and device_timestamp < to_timestamp(%ld)", timestampFrom, timestampTo);
    printf("Querying database: %s\n", query);
    res = PQexec(conn, query);

    _extract_tuples(res,  _createPMS5003TupleJson, output);

    free(query);
    return 0;
}


int insert_PMS5003_measurement(uint64_t device_timestamp, char * device_name, uint16_t pm10_standard, uint16_t pm25_standard, uint16_t pm100_standard) {
    char * conninfo;
    PGconn * conn;
    PGresult * res;

    _createConnInfoFromEnvs(&conninfo);

    conn = PQconnectdb(conninfo);
    free(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "connection error: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    
    char * query = malloc(MAX_PG_QUERY_SIZE);
    snprintf(query, MAX_PG_QUERY_SIZE, "INSERT INTO pms5003_measurements(%s, %s, %s, %s, %s) VALUES(to_timestamp(%ld), '%s', %d, %d, %d)",
        DEVICE_TIMESTAMP_FN,
        DEVICE_NAME_FN,
        PM10_STANDARD_FN,
        PM25_STANDARD_FN,
        PM100_STANDARD_FN,
        device_timestamp,
        device_name,
        pm10_standard,
        pm25_standard,
        pm100_standard);
    printf("Querying database: %s\n", query);

    res = PQexec(conn, query);
    free(query);

    
    ExecStatusType execStatus = PQresultStatus(res);
    if (execStatus ==  PGRES_COMMAND_OK) {
        printf("Inserted data correctly\n"); 
        return 0;
    } else {
        fprintf(stderr, "Error: got %d status from database\n", execStatus);
        return 1;
    }
}

CREATE TABLE IF NOT EXISTS pms5003_measurements (
	device_timestamp timestamp,
	device_name varchar(20),	
	pm10_standard smallint,
	pm25_standard smallint,
	pm100_standard smallint
);

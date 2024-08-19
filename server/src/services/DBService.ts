import ENVIRONMENTS from '../environments';
import pg from 'pg';
import MeteoServerError from '../utils/MeteoServerError';
import MeteoDataType from '../interfaces/MeteoDataType';


export default class DBService {
	constructor(){
		throw new MeteoServerError('DBService should not be instantiated, use static methods instead');
	}

	public static async insertMeasurement(dataType: MeteoDataType, data: Record<string, any>)	{
		let response = null;

		if (dataType === 'PMS5003') {	
			const { device_timestamp = null, device_name = null, pm10_standard = null, pm25_standard = null, pm100_standard = null } = data;

		if([device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard].includes(null)) {
			throw new MeteoServerError('One of the parameters is missing or distorted: device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard', 422);
		}	

		response = await DBService.query(`INSERT INTO pms5003_measurements(device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard) VALUES(to_timestamp(${device_timestamp}), '${device_name}', ${pm10_standard}, ${pm25_standard}, ${pm100_standard})`);
		}


		if (response === null) {
			throw new MeteoServerError(`Unsupported data type: ${dataType}`, 400);
		}
	}

	public static async getMeasurements(dataType: MeteoDataType, lookBackTimestamp: number) {
		const timestampThreshold = (Date.now() - lookBackTimestamp) / 1000;	

		let response = null;

		if (dataType === 'PMS5003') {
			response = await DBService.query(`SELECT * FROM pms5003_measurements WHERE device_timestamp > to_timestamp(${timestampThreshold})`);
		}

		if (response === null) {
			throw new MeteoServerError(`Unsupported data type: ${dataType}`);
		}

		return response.rows;
	}

	private static async query(query: string) {
		try {
			const client = new pg.Client({
				host: ENVIRONMENTS.POSTGRES_HOST,
				port: 5432,
				database: ENVIRONMENTS.POSTGRES_DB,
				user: ENVIRONMENTS.POSTGRES_USERNAME,
				password: ENVIRONMENTS.POSTGRES_PASSWORD,
			//	ssl: true
			})	
			
			await client.connect();
			const response = await client.query(query);
			await client.end();

			return response;
		} catch(e: any) {
			throw new MeteoServerError(`Database query: "${query}" failed with message: "${e.message}"`);
		}
	}
};

import ENVIRONMENTS from '../environments';
import pg from 'pg';
import MeteoServerError from '../utils/MeteoServerError';
import MeteoDataType from '../interfaces/MeteoDataType';


export default class DBService {
	constructor(){
		throw new MeteoServerError('DBService should not be instantiated, use static methods instead');
	}

	public static async getMeasurements(dataType: MeteoDataType, lookBackTimestamp: number) {
		const client = new pg.Client({
			host: ENVIRONMENTS.POSTGRES_HOST,
			port: 5432,
			database: ENVIRONMENTS.POSTGRES_DB,
			user: ENVIRONMENTS.POSTGRES_USER,
			password: ENVIRONMENTS.POSTGRES_PASSWORD,
			ssl: true
		})	

		try {
			await client.connect();

			// const response = await client.query('');
			await client.end();

			return 'dummy data';
		} catch(e: any) {
			throw new MeteoServerError(`Getting measurements from database failed with message ${e.message}`);
		}
	}
};

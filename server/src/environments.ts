import dotenv from 'dotenv';
import MeteoServerError from './utils/MeteoServerError';
dotenv.config();

const ENVIRONMENTS: Record<string, string | null> = {};
ENVIRONMENTS.THIS_API_PASSWORD = process.env.THIS_API_PASSWORD || null;
ENVIRONMENTS.THIS_API_USER = process.env.THIS_API_USER || null;
ENVIRONMENTS.POSTGRES_DB = process.env.POSTGRES_DB || null;
ENVIRONMENTS.POSTGRES_HOST = process.env.POSTGRES_HOST || null;
ENVIRONMENTS.POSTGRES_PASSWORD = process.env.POSTGRES_PASSWORD || null;
ENVIRONMENTS.POSTGRES_USERNAME = process.env.POSTGRES_USERNAME || null;

for (const key of Object.keys(ENVIRONMENTS)) {
	if (ENVIRONMENTS[key] === null) {
		throw new MeteoServerError('Some environment variables are missing, terminating app');
	}
}

export default ENVIRONMENTS;

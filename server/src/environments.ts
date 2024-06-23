import dotenv from 'dotenv';
dotenv.config();

const ENVIRONMENTS: Record<string, string | undefined> = {};
ENVIRONMENTS.THIS_API_PASSWORD = process.env.THIS_API_PASSWORD;
ENVIRONMENTS.THIS_API_USER = process.env.THIS_API_USER;

for (const key of Object.keys(ENVIRONMENTS)) {
	if (ENVIRONMENTS[key] === undefined) {
		throw new Error('Some environment variables are missing, terminating app');
	}
}

export default ENVIRONMENTS;

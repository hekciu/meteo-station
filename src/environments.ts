import dotenv from 'dotenv';
dotenv();

const ENVIRONMENTS: Record<string, string> = {};
ENVIRONMENTS.THIS_API_PASSWORD = process.env.THIS_API_PAORD;
ENVIRONMENTS.THIS_API_USER = process.env.THIS_API_USER;

if (!thisApiPassword || !thisApiUser) {
	throw new Error('Some environment variables are missing, terminating app');
}

export default ENVIRONMENTS;

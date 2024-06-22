const ENVIRONMENTS: Record<string, string> = {};
ENVIRONMENTS.thisApiPassword = process.env.THIS_API_PASSWORD;
ENVIRONMENTS.thisApiUser = process.env.THIS_API_USER;

if (!thisApiPassword || !thisApiUser) {
	throw new Error('Some environment variables are missing, terminating app');
}

export default ENVIRONMENTS;

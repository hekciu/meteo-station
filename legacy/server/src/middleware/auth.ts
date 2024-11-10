import ENVIRONMENTS from '../environments';
import { Request, Response, NextFunction } from 'express';
import MeteoServerError from '../utils/MeteoServerError';

const auth = (req: Request, res: Response, next: NextFunction) => {
	const authString = Buffer.from(`${ENVIRONMENTS.THIS_API_USER}:${ENVIRONMENTS.THIS_API_PASSWORD}`).toString('base64');

	const userAuthHeader = req.get('Authorization');

	if (!userAuthHeader || !userAuthHeader.startsWith('Basic ')) {
		throw new MeteoServerError('Missing or distorted auth header', 400);
	}

	if (userAuthHeader.slice(6) !== authString) {
		throw new MeteoServerError('Authentication failed', 403);
	}

	next();
}

export default auth;

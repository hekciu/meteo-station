import { Request, Response, NextFunction } from 'express';
import MeteoServerError from '../utils/MeteoServerError';

const errorHandler = (err: any, req: Request, res: Response, next: NextFunction) => {
	err = typeof err === 'object' ? err : { message: null };

	console.error(`Error occured with message: ${err.message}`);

	if(err instanceof MeteoServerError) {
		return res.status(err.getStatusCode()).send(err.message);
	}

	res.status(500).send(err.message || 'Something went terribly wrong...');
}

export default errorHandler;

import { Request, Response, NextFunction } from 'express';
import MeteoServerError from '../utils/MeteoServerError';

export default class DataController {
	static async insertData(req: Request, res: Response, next: NextFunction) {
		const { data = null } = req.body;
		
		if (!data) {
			next(new MeteoServerError(`Missing 'data' field in request`, 400));
		}


		console.log(JSON.stringify(data));
	}
}

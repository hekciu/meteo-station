import { Request, Response, NextFunction } from 'express';
import MeteoServerError from '../utils/MeteoServerError';

export default class DataController {
	static async insertData(req: Request, res: Response, next: NextFunction) {
		try {
			const { data = null } = req.body;
		
			if (!data) {
				throw new MeteoServerError(`Missing 'data' field in request`, 400);
			}


			console.log(JSON.stringify(data));
			res.status(200).send("git");
		} catch (e: unknown) {
			next(e);
		}
	}
}

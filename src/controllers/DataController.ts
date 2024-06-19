import { Request, Response, NextFunction } from 'express';

export default class DataController {
	static async insertData(req: Request, res: Response, next: NextFunction) {
		console.log(req)
		const { data = null } = req.body;
		
		if (!data) {
			throw new Error(`Missing 'data' field in request`);
		}


		console.log(JSON.stringify(data));
	}
}

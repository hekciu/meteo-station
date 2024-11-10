import { Request, Response, NextFunction } from 'express';
import MeteoServerError from '../utils/MeteoServerError';
import DBService from '../services/DBService';
import MeteoDataType from '../interfaces/MeteoDataType';


export default class DataController {
	constructor() {
		throw new MeteoServerError('DataController should not be instantiated, use static methods');
	}

	static async insertData(req: Request, res: Response, next: NextFunction) {
		const { data = null, dataType = null } = req.body;
	
		if (!data || !dataType) {
			throw new MeteoServerError(`Missing 'data' or 'dataType' field in request`, 400);
		}
		
		await DBService.insertMeasurement(dataType as MeteoDataType, data as Record<string, any>);
		console.log(JSON.stringify(data));
		res.status(200).send("git");
	}

	static async getMeasurements(req: Request, res: Response, next: NextFunction) {
		const { dataType = null, lookBackTimestamp = null } = req.query;	

		if (!dataType || !lookBackTimestamp) {
			new MeteoServerError(`Missing 'dataType' or 'lookBackTimestamp' query param`, 400);
		}

		const data = await DBService.getMeasurements(dataType as MeteoDataType, Number(lookBackTimestamp));

		res.status(200).send(data);
	}
}

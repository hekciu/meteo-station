import ENVIRONMENTS from '../environments';
import { Request, Response, NextFunction } from 'express';

const auth = (req: Request, res: Response, next: NextFunction) => {
	console.log(ENVIRONMENTS);	
	next();
}

export default auth;

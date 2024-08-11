import ENVIRONMENTS from '../environments';
import pg from 'pg';
import MeteoServerError from '../utils/MeteoServerError';


export default class DBService {
	constructor(){
		throw new MeteoServerError('DBService should not be instantiated, use static methods instead');
	}
};

import express from 'express';
import DataController from '../controllers/DataController';
import auth from '../middleware/auth';
import errorHandler from '../middleware/errorHandler';


const privateRouter = express.Router();

privateRouter.use(auth);

privateRouter.post('/data', async (req, res, next) => {
	await DataController.insertData(req, res, next);
})

privateRouter.use(errorHandler);

export default privateRouter;

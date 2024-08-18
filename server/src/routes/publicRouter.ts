import express from 'express';
import DataController from '../controllers/DataController';
import errorHandler from '../middleware/errorHandler';

const publicRouter = express.Router();

publicRouter.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

publicRouter.get('/measurements', DataController.getMeasurements);

publicRouter.use(errorHandler);

export default publicRouter;

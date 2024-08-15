import express from 'express';
import DataController from '../controllers/DataController';

const publicRouter = express.Router();

publicRouter.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

publicRouter.get('measurements', DataController.getMeasurements);

export default publicRouter;

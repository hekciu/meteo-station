import express from 'express';
import DataController from '../controllers/DataController';
import auth from '../middleware/auth';

const privateRouter = express.Router();

privateRouter.use(auth);

privateRouter.post('/data', (req, res, next) => {
	DataController.insertData(req, res, next);
})


export default privateRouter;

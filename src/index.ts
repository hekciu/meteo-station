import express from 'express';
import errorHandler from './middleware/errorHandler';
import auth from './middleware/auth';
import DataController from './controllers/DataController';
import { PORT } from './constants';

const app = express();

app.use(express.json());

app.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

app.use(auth);

app.post('/data', (req, res, next) => {
	DataController.insertData(req, res, next);
})

app.use(errorHandler);

app.listen(PORT, () => console.info(`Listening on ${PORT}`));

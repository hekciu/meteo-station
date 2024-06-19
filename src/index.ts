import express from 'express';

import DataController from './controllers/DataController';
import { PORT } from './constants';

const app = express();

app.use(express.json());

app.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

app.post('/data', (req, res, next) => {
	DataController.insertData(req, res, next);
})

app.listen(PORT, () => console.info(`Listening on ${PORT}`));

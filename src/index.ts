import express from 'express';

import { PORT } from './constants';

const app = express();

app.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

app.post('/pmsData', (req, res) => {
	
})

app.listen(PORT, () => console.info(`Listening on ${PORT}`));

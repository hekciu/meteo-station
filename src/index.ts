import express from 'express';

import { PORT } from './constants';

const app = express();

app.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

app.listen(PORT, () => console.log(`Listening on ${PORT}`));

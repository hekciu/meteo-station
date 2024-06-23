import express from 'express';

const publicRouter = express.Router();

publicRouter.get('/healthcheck', (_, res) => {
	res.status(200).send('OK');
})

export default publicRouter;

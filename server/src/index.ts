import express from 'express';
import DataController from './controllers/DataController';
import { PORT } from './constants';
import publicRouter from'./routes/publicRouter';
import privateRouter from'./routes/privateRouter';


const app = express();

app.use(express.json());

app.use(publicRouter);
app.use(privateRouter);


app.listen(PORT, () => console.info(`Listening on ${PORT}`));

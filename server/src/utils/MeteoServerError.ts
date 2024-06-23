

export default class MeteoServerError extends Error {
	private statusCode: number;

	constructor(msg: string, code?: number) {
		super(msg);
		this.statusCode = code || 500;
	}	


	public getStatusCode(): number {
		return this.statusCode;
	}
}

source ./env.sh

#echo $POSTGRES_DB
#echo $POSTGRES_HOST
#echo $POSTGRES_PASSWORD
#echo $POSTGRES_USERNAME

for SQL_FILE in ./sql/*.sql; do
	if [[ "$SQL_FILE" =~ ^.*[0-9]+_[0-9]+_[0-9]+\.sql$ ]] 
	then
		echo "Processing $SQL_FILE"
		PGPASSWORD="$POSTGRES_PASSWORD" psql -h "$POSTGRES_HOST" -U "$POSTGRES_USERNAME" -d "$POSTGRES_DB" < "$SQL_FILE"
	fi
done

sh clean.sh

docker build -t compile-saturn:1.0.0 . 
docker run -d --rm --name saturn-paeng -v .:/joengine/samples/saturn-paeng  compile-saturn:1.0.0 
docker exec saturn-paeng sh compile.sh
docker stop saturn-paeng

rm game.cue
rm game.iso

docker build -t compile-saturn:1.0.0 . 
docker run -d --rm --name saturn-paeng  compile-saturn:1.0.0
docker cp saturn-paeng:/joengine/samples/saturn-paeng/game.iso .
docker cp saturn-paeng:/joengine/samples/saturn-paeng/game.cue .
docker stop saturn-paeng

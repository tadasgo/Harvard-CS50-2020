SELECT movies.title, ratings.rating FROM movies
JOIN ratings on movies.id = ratings.movie_id
WHERE year = 2010
ORDER BY 2 DESC, 1;
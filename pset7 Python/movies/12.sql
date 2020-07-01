SELECT movies.title FROM people
JOIN stars on people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE people.name = 'Johnny Depp'
INTERSECT
SELECT movies.title FROM people
JOIN stars on people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE people.name = 'Helena Bonham Carter';
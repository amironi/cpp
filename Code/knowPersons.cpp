

person maybeFamous(person i, person j) { 
  return know(i,j) ? j : i; 
}

person findFamous(List<person> people)
{

	if(people.size() == 0) 
		return person();

    person famous = people.front();
    
    for(auto& i : people)
      famous = maybeFamous(i, famous );

    for(auto& i : people){
      if( 
        maybeFamous(i, famous) != famous || 
        maybeFamous(famous, i) != famous )
          return person(); 
    }

    return famous;
}


[p0, p1, p2, ...., pN]

bool know(i, j) returns true if I knows J, false otherwise (this is not symetrical) O(1)

person findFamous(List<person> people): 

famous is: 
  1) Everyone else knows the famous person AND
  2) The famous person doesn't know anyone else

Expected solution is O(n)

know(i, j) -> true -> i knows j -> i cannot be famous, j can be famous  
              false -> i does not know j -> i can be famous, j cannot be famous

candidate = person();

if(know(candidate,i) && (!know(i,candidate)
  candidate = i;
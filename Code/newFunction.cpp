template<typename A, typename... Args>
A* mynew(const Args&&... args)
{

  A* m = malloc(sizeof(A));
  
  try {
    new(m) A(forward<Args>(args)...);
  }
  catch(...)
  {
    free(m);
  }
  
  return m;
}

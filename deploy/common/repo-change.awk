BEGIN {}  
{
  if(index($0,"Changelog")>=1) 
    printf "%s\n",REPO; 
  else if (index($0,"=========")>=1)
    printf "%s\n",SUB; 
  else
    print $0;
}
END{}     

#include "modio_c.h"

void onAddModYoutubeLinks(void *object, ModioResponse response)
{
  bool *wait = object;
  printf("Edit mod logo response: %i\n", response.code);
  if (response.code == 201)
  {
    printf("Image downloaded successfully!\n");
  }
  *wait = false;
}

int main(void)
{
  modioInit(MODIO_ENVIRONMENT_TEST, 7, (char *)"e91c01b8882f4affeddd56c96111977b");

  bool wait = true;

  // Let's start by requesting a single mod
  printf("Please enter the mod id: \n");
  u32 mod_id;
  scanf("%i", &mod_id);
  
  char **youtube_links_array = (char **)malloc(1);
  youtube_links_array[0] = (char *)malloc(100);
  strcpy(youtube_links_array[0], "https://www.youtube.com/watch?v=dQw4w9WgXcQ\0");

  printf("Getting mod...\n");
  modioAddModYoutubeLinks(&wait, mod_id, youtube_links_array, 1, &onAddModYoutubeLinks);
  
  while (wait)
  {
    modioProcess();
  }

  modioShutdown();

  printf("Process finished\n");

  return 0;
}

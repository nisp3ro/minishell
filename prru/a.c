int main(int argc, char **argv)
{
	int i = 0;

	if (argc > 1)
		while(argv[i])
		{
			printf("%s", argv[i]);
			printf("\n");
			i++;
		}
	return(0);
}

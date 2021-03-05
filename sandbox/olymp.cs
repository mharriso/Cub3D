using System;

public sealed class UserHandler
{
	public crypt(int[] arr, int x)
	{
		for (int i = 0; i < arr.Length; i++)
		{
			arr[i] ^= x;
			Console.Write($"{i} = {arr[i]}, ");
		}
		Console.Write("\n");
	}
}

public class Program
{
	int Main()
	{
		int[] arr = new int[]
		{
			1, 2
		}

		var handler = new UserHandler();
		handler.crypt();
	}
}

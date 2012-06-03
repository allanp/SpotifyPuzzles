package test.spotify;

import java.util.Arrays;

public class BestBefore {
	private final static int MMIN_YEAR = 100;
	private final static int MIN_YEAR = 2000;
	private final static int MAX_YEAR = 2999;
	
	private final static String Spliter = "/";
	private final static String IS_ILLEGAL_MESSAGE = "is illegal";
	private final static int MAX_MONTH = 12;
	private final static int MAX_DAY = 31;
	private final static int DAYS[] = {0, -3, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0 }; // days reference to 31
	
	private boolean isLeapYear(int year){
		return !((year % 4 > 0) || ( year % 100 == 0 && year % 400 > 0)); 
	}
	
	private boolean isValidDate(int[] dateInArray){
		return isValidDate(dateInArray[0], dateInArray[1], dateInArray[2]);
	}
	
	private boolean isValidDate(int year, int month, int day){
		year += (year <= MMIN_YEAR) ? MIN_YEAR : 0;
		if(year < MIN_YEAR || year > MAX_YEAR || 
		   month <= 0 || month > MAX_MONTH || day <= 0)
			return false;
		if(day > MAX_DAY + DAYS[month - 1] + ((isLeapYear(year) ? 1 : 0)))
			return false;
		return true;
	}
	
	private int[] foramtToNumbers(String dateString){
		int[] numbers = new int[3];
		int i = 0;
		for(String seg : dateString.split(Spliter))
			numbers[i++] = Integer.parseInt(seg);
		return numbers;
	}

	private int[] reorderArray(int[] arr, int index){
		int x = arr[0], y = arr[1], z = arr[2];
		switch(index){ // x,y,z; y,x,z; y,z,x; z,x,y; z,y,x
			case 0:
				arr[0] = x; arr[1] = z; arr[2] = y;
				break;
			case 1:
				arr[0] = z; arr[1] = x; arr[2] = y;
				break;
			case 2:
				arr[0] = x; arr[1] = z; arr[2] = y;
				break;
			case 3:
				arr[0] = y; arr[1] = z; arr[2] = x;
				break;
			case 4:
				arr[0] = x; arr[1] = z; arr[2] = y;
				break;
			default:
				break;
		}
		return arr;
	}
		
	private String getBestBefore(String dateString){
		int dateInArray[] = foramtToNumbers(dateString);
		Arrays.sort(dateInArray);
		boolean isValid = false;
		int index = 0;
		while(!(isValid = isValidDate(dateInArray))){
			dateInArray = reorderArray(dateInArray, index++);
			if(index == 5) break;
		}
		if(isValid){
			if(dateInArray[0] <= MMIN_YEAR)
				dateInArray[0] += MIN_YEAR;
			return String.format("%d-%02d-%02d", dateInArray[0], dateInArray[1], dateInArray[2]);
		} else {
			return IS_ILLEGAL_MESSAGE;
		}
	}

	private void showUsage(){
		System.err.print("\n Usage: \n" + 
						 "   $ BestBefore 1/12/13 02/28/00\n\n");
	}

	public static void main(String[] args) {
		BestBefore bb = new BestBefore();
		if(args.length ==0){
			bb.showUsage();
		}
		else{
			for(String arg : args)
				System.out.println(bb.getBestBefore(arg));
		}
	}
}

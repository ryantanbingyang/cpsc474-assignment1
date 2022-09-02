TestCribbage:
	javac -cp cpsc474_cribbage.jar:. TestCribbage.java
	echo "#!/bin/bash" > TestCribbage
	echo "java -cp cpsc474_cribbage.jar:. TestCribbage \"\$$@\"" >> TestCribbage
	chmod u+x TestCribbage

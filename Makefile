FFMPEG_LIB = `pkg-config --libs libavformat libavcodec libswscale libavutil`
FFMPEG_CFLAGS = `pkg-config --cflags libavformat libavcodec libswscale libavutil`
 
rtspVideoRecord: main.cpp json_reader.o json_value.o json_writer.o readConfig.o rtspVideoRecord.o utility.o
	g++ -o ./rtspVideoRecord main.cpp rtspVideoRecord.o readConfig.o utility.o \
	    json_reader.o json_value.o json_writer.o -lpthread \
	    `pkg-config opencv --libs --cflags` $(FFMPEG_CFLAGS) $(FFMPEG_LIB)
	rm *.o
json_reader.o: ./json/json_reader.cpp
	g++ -o json_reader.o -c ./json/json_reader.cpp
json_value.o: ./json/json_value.cpp
	g++ -o json_value.o -c ./json/json_value.cpp
json_writer.o: ./json/json_writer.cpp
	g++ -o json_writer.o -c ./json/json_writer.cpp
readConfig.o:  readConfig.h readConfig.cpp
	g++ -o readConfig.o -c readConfig.cpp  
rtspVideoRecord.o: rtspVideoRecord.h rtspVideoRecord.cpp
	g++ -o rtspVideoRecord.o -c rtspVideoRecord.cpp
utility.o: utility.h utility.cpp
	g++ -o utility.o -c utility.cpp
clean:
	rm ./rtspVideoRecord

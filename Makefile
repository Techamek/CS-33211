compile:
	gcc producer.c -pthread -lrt -o producer
	gcc consumer.c -pthread -lrt -o consumer

run:
	./producer & ./consumer &

clean:
	rm ./producer
	rm ./consumer

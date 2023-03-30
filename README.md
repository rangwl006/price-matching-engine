# Price Matching Engine(PME) with TCP/IP

## Environment setup
The PME server and demo client runs on and is also built by Docker Linux.

1. Install docker-compose on host machine
2. Clone the following repository `git@github.com:rangwl006/generic-docker-setup.git`
3. Build the image using `docker-compose build` in the toplevel folder the cloned repository.

## Running the PME

1. Clone the following repository using `git clone git@github.com:rangwl006/price-matching-engine.git` into another 
   folder
2. To build the tcp-pme and generator-client applications, run `docker-compose run --rm build-application`
3. Open a terminal to run the TCP-PME Server using `docker-compose up start-tcp-server`
4. Open a separate terminal to run the Generator-Client application using `docker-compose run --rm start-tcp-client`

## Input data 

### Data format
The client will take in a plain `.txt` file containing data on the price, quantity, id and buy/sell order type, each 
separated by a space, and the last piece of data should end with a `#`. `#` is what is used to define the end of 
each message and is needed in case of partial reads.

schema: | id | price | quantity | buy(0)/sell(1) |terminating character

sample message: 1 2 20 1#

### Defining you own data
1. To feed in your own data, create a `.txt` file like that in `resources/data.txt`, whilst following the format 
defined above.
2. In `resources/settings`, under the `client` key, change the file path of the data file.

## Troubleshooting

1. Occasionally, the tcp-server will not always produce the correct IP address for the client to connect to. In such 
   a case, we need to find the IP address of the server, which can be found using `docker inspect start-tcp-server | 
   grep IPAddress`. After doing so, change the `ip` field under the `client` key in `resources/settings`.

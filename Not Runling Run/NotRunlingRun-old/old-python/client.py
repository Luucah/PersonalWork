import asyncio

import websockets


async def consumer(websocket):
    print("Consuming...")
    async for message in websocket:
        print(message)

async def producer(websocket):
    while True:
        message = input('Send a message: ')
        await websocket.send(message)
        print("Sent")
        await websocket.ping()
        print("pinged")

async def hello():
    uri = "ws://localhost:8765"
    async with websockets.connect(uri) as websocket:
        name = input("What's your name? ")

        await websocket.send(name)
        print(f">>> {name}")

        greeting = await websocket.recv()
        print(f"<<< {greeting}")

        consumer_task = asyncio.create_task(consumer(websocket))
        producer_task = asyncio.create_task(producer(websocket))
        done, pending = await asyncio.wait(
            [consumer_task, producer_task],
            return_when=asyncio.FIRST_COMPLETED,
        )
        for task in pending:
            print("Canceling")
            task.cancel()

if __name__ == "__main__":
    asyncio.run(hello())

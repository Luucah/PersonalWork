
import asyncio
import websockets

async def consumer(websocket):
    print("Consuming...")
    async for message in websocket:
        print(message)

async def hello():
    uri = "ws://localhost:8765"
    async with websockets.connect(uri) as websocket:
        await websocket.send("consumer")
        greeting = await websocket.recv()
        print(f"Got greeting: {greeting}")
        await consumer(websocket)
        print("All done")

if __name__ == "__main__":
    asyncio.run(hello())

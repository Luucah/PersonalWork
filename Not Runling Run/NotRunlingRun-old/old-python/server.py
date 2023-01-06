
import asyncio
import websockets

CONNECTIONS = set()
NAMES = {}

async def broadcast(message):
    for c in CONNECTIONS:
        await c.send(message)

async def hello(websocket):
    name = await websocket.recv()
    print(f"<<< {name}")

    greeting = f"Hello {name}!"

    await websocket.send(greeting)
    print(f">>> {greeting}")

    NAMES[websocket] = name

    CONNECTIONS.add(websocket)
    try:
        # await websocket.wait_closed()
        await websocket.send("first message...")
        await chatroom(websocket)
    finally:
        CONNECTIONS.remove(websocket)

async def process(name, message):
    fullMessage = f"[{name}] {message}"
    print(f"{fullMessage} | to {len(CONNECTIONS)} clients")
    await websockets.broadcast(CONNECTIONS, fullMessage)
    print("Finished sending message")


async def chatroom(websocket):
    async for message in websocket:
        name = NAMES[websocket]
        await process(name, message)
    print("Finished")

async def main():
    async with websockets.serve(hello, "localhost", 8765):
        await asyncio.Future()  # run forever
        # await chatroom(websocket)

if __name__ == "__main__":
    asyncio.run(main())
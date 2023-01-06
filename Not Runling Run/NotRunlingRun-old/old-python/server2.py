
import asyncio
import websockets

import asyncio
import websockets

async def hello(websocket, path):
    name = await websocket.recv()
    print("< {}".format(name))

    greeting = "Hello {}!".format(name)
    await websocket.send(greeting)
    print("> {}".format(greeting))

start_server = websockets.serve(hello, 'localhost', 8765)
# asyncio.run(start_server)

# asyncio.get_running_loop().run_until_complete(start_server)
# asyncio.get_running_loop().run_forever()

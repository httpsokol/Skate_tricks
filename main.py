import asyncio
from bleak import BleakClient

ADDRESS = "3652F7D3-0006-5771-47AB-439AA4F7E33D"

CHAR_UUID = "87654321-4321-4321-4321-ba0987654321"


def callback(sender, data):
    print("RX:", data.decode())


async def main():
    async with BleakClient(ADDRESS) as client:

        print("Connected:", client.is_connected)

        await client.start_notify(CHAR_UUID, callback)

        while True:
            await asyncio.sleep(1)


asyncio.run(main())
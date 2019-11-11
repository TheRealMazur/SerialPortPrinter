# SerialPortPrinter
Small programm made for sending receipt printing commands to **emar printo 57t** receipt printer.

It also shows receipt data preview in a table.

Data must be provided in a **.rct** file containing a JSON text structure.

Example file content:

```javascript
{
  "data": [
    "1#e",
    "1$d",
    "0$h",
    "1;0;0;$lOrder 1<CR>1<CR>A/0.01/0.01/",
    "1;1$b0.01/Credit Card<CR>",
    "1;$ePark Cafe<CR>0/0.01",
    "12$z",
    "28;0$z"
  ],
  "preview": {
    "1": {
      "name": "Order 1",
      "amount": 1,
      "price": "0.01",
      "total": "0.01"
    }
  }
}
```

The programm automatically converts **"\<CR\>"** to **"\r"** inside the commads.

It also calculates the chechsum for every command and wraps the commands into a structure defined in the printers protocol:

**ESC P \<data goes here\> \<checksum> ESC \\**

Where **ESC** is ASCII code **0x1b**

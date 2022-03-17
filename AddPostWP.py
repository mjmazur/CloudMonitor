from wordpress_xmlrpc import Client, WordPressPost
from wordpress_xmlrpc.compat import xmlrpc_client
from wordpress_xmlrpc.methods import media, posts

client = Client('http://www.orebits.ca/xmlrpc.php', 'cloudbot', '7IfCA^zcJtDz8ccM#I91HEn*')

# set to the path to your file
filename = './test-1.png'

# prepare metadata
data = {
        'name': 'test-1.png',
        'type': 'image/png',  # mimetype
}

# read the binary file and let the XMLRPC library encode it into base64
with open(filename, 'rb') as img:
        data['bits'] = xmlrpc_client.Binary(img.read())

data['overwrite']=True
response = client.call(media.UploadFile(data))
# response == {
#       'id': 6,
#       'file': 'picture.jpg'
#       'url': 'http://www.example.com/wp-content/uploads/2012/04/16/picture.jpg',
#       'type': 'image/jpeg',
# }
attachment_id = response['id']

print(attachment_id)
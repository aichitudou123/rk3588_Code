from bot import bot_factory
from config import conf
from common import const

"""
const.py中
OPEN_AI = openAI
CHATGPT = chatGPT
BAIDU = baidu
"""
class Bridge(object):
    def __init__(self):
        pass

    def fetch_reply_content(self, query, context):
        bot_type = const.CHATGPT 
        model_type = conf().get("model")
        #use gpt-3.5-turbo(config.json中配置)
        if model_type in ["gpt-3.5-turbo", "gpt-4", "gpt-4-32k"]:
            bot_type = const.CHATGPT
        elif model_type in ["text-davinci-003"]:
            bot_type = const.OPEN_AI
        #bot_type = chatGPT
        return bot_factory.create_bot(bot_type).reply(query, context)


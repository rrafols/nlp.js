/*
 * Copyright (c) AXA Group Operations Spain S.A.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

const { uuid } = require('@nlpjs/core');

class Session {
  constructor(connector = {}, activity = {}) {
    this.activity = activity;
    this.connector = connector;
    this.bot = this.connector.container.get('bot');
    if (!this.connector.settings) {
      this.connector.settings = {};
    }
    this.type = 'message';
    this.serviceUrl = this.activity.serviceUrl;
    this.channelId =
      this.activity.channelId || this.connector.settings.tag || 'emulator';
    this.conversation = {
      id: activity.conversation ? activity.conversation.id : 'conversation',
    };
    this.text = this.activity.text;
    this.recipient = this.activity.from;
    this.inputHint = this.activity.inputHint || 'acceptingInput';
    this.replyToId = this.activity.id;
    this.id = uuid();
    this.from = {
      id: process.env.BACKEND_ID || this.connector.settings.tag || 'emulator',
      name:
        process.env.BACKEND_NAME || this.connector.settings.tag || 'emulator',
    };
  }

  say(message, context) {
    let outputMessage = message;
    if (context) {
      const template = this.bot
        ? this.bot.container.get('Template')
        : undefined;
      if (template) {
        outputMessage = template.compile(message, context);
      }
    }
    this.connector.say(this.activity, outputMessage);
  }

  beginDialog(context, name) {
    this.bot.dialogManager.beginDialog(context.dialogStack, name);
  }

  endDialog(context) {
    this.bot.dialogManager.endDialog(context.dialogStack);
  }
}

module.exports = Session;

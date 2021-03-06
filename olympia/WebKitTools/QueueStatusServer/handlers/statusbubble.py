# Copyright (C) 2009 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import operator

from google.appengine.ext import webapp
from google.appengine.ext.webapp import template

from model.attachment import Attachment
from model.workitems import WorkItems
from model.queues import queues, name_with_underscores


class StatusBubble(webapp.RequestHandler):
    # FIXME: This list probably belongs as part of a Queue object in queues.py
    # Arrays are bubble_name, queue_name
    _queues_to_display = [
        ["style", "style-queue"],
        ["cr-linux", "chromium-ews"],
        ["gtk", "gtk-ews"],
        ["qt", "qt-ews"],
        ["mac", "mac-ews"],
        ["win", "win-ews"],
    ]

    # This asserts that all of the queues listed above are valid queue names.
    assert(reduce(operator.and_, map(lambda name_pair: name_pair[1] in queues, _queues_to_display)))

    def _build_bubble(self, queue_name_pair, attachment):
        bubble_name = queue_name_pair[0]
        queue_name = queue_name_pair[1]

        queue_status = attachment.status_for_queue(queue_name)
        bubble = {
            "name": bubble_name,
            "attachment_id": attachment.id,
            "queue_position": attachment.position_in_queue(queue_name),
            "state": attachment.state_from_queue_status(queue_status) if queue_status else "none",
            "status": queue_status,
        }
        return bubble

    def get(self, attachment_id):
        attachment = Attachment(int(attachment_id))
        bubbles = [self._build_bubble(name_pair, attachment) for name_pair in self._queues_to_display]
        template_values = {
            "bubbles": bubbles,
        }
        self.response.out.write(template.render("templates/statusbubble.html", template_values))

pragma Strict

import TestTypes
import QtQml

Person {
    id: self

    area {
        width: 19
        height: 199
    }

    property list<int> ints: [4, 3, 2, 1]

    property outer recursive
    property Person shadowable: Person {
        area.width: self.area.width
        area2.height: self.area2.height
    }

    Component.onCompleted: {
        area.width = 16
        area2.height = 17

        self.area.x = 4
        self.area2.y = 5

        shadowable.area.x = 40
        shadowable.area2.y = 50

        self.recursive.inner.i = 99;

        self.ints[0] = 12;
        ints[1] = 22;
        ints[6] = 33;
    }

    property int inner: recursive.inner.i
}

import {
    useState
} from 'react';


import {
    View,
    Button,
} from 'react-native';



type DataView = 'chart' | 'map';


export default function Measurements() {
    const [dataView, setDataView] = useState<DataView>('chart');

    return (
        <View style={{}}>
            <Button title={dataView} onPress={() => {
                if (dataView === 'chart') {
                    setDataView('map');
                } else {
                    setDataView('chart');
                }
            }}/>
        </View>
    )
};

